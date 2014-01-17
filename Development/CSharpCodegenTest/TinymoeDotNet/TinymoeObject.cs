using System;
using System.Collections.Generic;
using System.Linq;

namespace TinymoeDotNet
{
    public class TinymoeObject
    {
        private static readonly Dictionary<Tuple<Type, string>, TinymoeObject> extensions = new Dictionary<Tuple<Type, string>, TinymoeObject>();
        private Dictionary<string, TinymoeObject> fields = new Dictionary<string, TinymoeObject>();

        public static void SetExtension(Type type, string name, TinymoeObject value)
        {
            extensions[Tuple.Create(type, name)] = value;
        }

        public void SetField(string name, TinymoeObject value)
        {
            fields[name] = value;
        }

        public TinymoeObject GetField(string name)
        {
            TinymoeObject value = null;
            if (fields.TryGetValue(name, out value))
            {
                return value;
            }

            Type type = GetType();
            while (type != null)
            {
                if (extensions.TryGetValue(Tuple.Create(type, name), out value))
                {
                    return value;
                }
                type = type.BaseType;
            }

            throw new ArgumentOutOfRangeException("name");
        }
    }

    public class TinymoeBoolean : TinymoeObject
    {
        public bool Value { get; private set; }

        public TinymoeBoolean(bool value)
        {
            this.Value = value;
        }
    }

    public class TinymoeInteger : TinymoeObject
    {
        public int Value { get; private set; }

        public TinymoeInteger(int value)
        {
            this.Value = value;
        }
    }

    public class TinymoeFloat : TinymoeObject
    {
        public double Value { get; private set; }

        public TinymoeFloat(double value)
        {
            this.Value = value;
        }
    }

    public class TinymoeString : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeString(string value)
        {
            this.Value = value;
        }
    }

    public class TinymoeSymbol : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeSymbol(string value)
        {
            this.Value = value;
        }
    }

    public class TinymoeArray : TinymoeObject
    {
        public TinymoeObject[] Elements { get; private set; }

        public TinymoeArray(int length)
        {
            this.Elements = new TinymoeObject[length];
        }

        public TinymoeArray(IEnumerable<TinymoeObject> values)
        {
            this.Elements = values.ToArray();
        }
    }

    public class TinymoeFunction : TinymoeObject
    {
        public Action<TinymoeObject[]> Handler { get; private set; }

        public TinymoeFunction(Action<TinymoeObject[]> value)
        {
            this.Handler = value;
        }
    }

    public class TinymoeOperations
    {
        public static readonly Dictionary<string, TinymoeObject> ExternalFunctions = new Dictionary<string, TinymoeObject>();

        public static TinymoeObject Positive(TinymoeObject a)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Negative(TinymoeObject a)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Not(TinymoeObject a)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Concat(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Add(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Sub(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Mul(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Div(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject IntDiv(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Mod(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject LT(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject GT(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject LE(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject GE(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject EQ(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject NE(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject And(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Or(TinymoeObject a, TinymoeObject b)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject Invoke(TinymoeObject function, TinymoeObject[] arguments)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject CastToBoolean(TinymoeObject a)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject CastToInteger(TinymoeObject a)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject CastToFloat(TinymoeObject a)
        {
            throw new NotImplementedException();
        }

        public static TinymoeObject CastToString(TinymoeObject a)
        {
            throw new NotImplementedException();
        }
    }
}
